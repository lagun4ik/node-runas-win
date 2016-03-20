#include <node.h>
#include <WinSock2.h>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include "nan.h"

namespace runasWindows {

	using v8::Function;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Handle;
	using v8::Local;
	using v8::Null;
	using v8::Object;
	using v8::Array;
	using v8::String;
	using v8::Value;
	using v8::Exception;
	using v8::FunctionTemplate;

	bool GetProperty(Local<Object> obj, const char* key, Local<Value>* value) {
		return Nan::Get(obj, Nan::New<String>(key).ToLocalChecked()).ToLocal(value);
	}

	std::string QuoteCmdArg(const std::string& arg) {
		if (arg.size() == 0)
			return arg;

		if (arg.find_first_of(" \t\"") == std::string::npos)
			return arg;

		if (arg.find_first_of("\"\\") == std::string::npos)
			return std::string("\"") + arg + '"';

		std::string quoted;
		bool quote_hit = true;
		for (size_t i = arg.size(); i > 0; --i) {
			quoted.push_back(arg[i - 1]);

			if (quote_hit && arg[i - 1] == '\\') {
				quoted.push_back('\\');
			}
			else if (arg[i - 1] == '"') {
				quote_hit = true;
				quoted.push_back('\\');
			}
			else {
				quote_hit = false;
			}
		}

		return std::string("\"") + std::string(quoted.rbegin(), quoted.rend()) + '"';
	}

	std::string GetArguments(Local<Value> arr) {
		auto array = v8::Handle<v8::Array>::Cast(arr);

		std::string arguments;

		for (auto i = 0; i < array->Length(); i++)
		{
			arguments += QuoteCmdArg(*String::Utf8Value(array->Get(i))) + " ";
		}

		return arguments;		
	}

	void Run(const FunctionCallbackInfo<Value>& args) {
		auto isolate = args.GetIsolate();

		if (args.Length() < 4) {
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}

		if (!args[0]->IsString() || !args[1]->IsArray() || !args[2]->IsObject() || !args[3]->IsFunction()) {
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
			return;
		}

		Local<Value> option;
		SHELLEXECUTEINFO shExInfo = { 0 };
		
		if (GetProperty(args[2]->ToObject(), "hide", &option) && option->BooleanValue())
		{
			shExInfo.nShow = SW_HIDE;
		}
		else
		{
			shExInfo.nShow = SW_SHOW;
		}

		if (GetProperty(args[2]->ToObject(), "admin", &option) && option->BooleanValue())
		{
			shExInfo.lpVerb = _T("runas");
		}
		else
		{
			shExInfo.lpVerb = _T("open");
		}

		std::string programm = *String::Utf8Value(args[0]);
		auto programmParams = GetArguments(args[1]);
		
		shExInfo.cbSize = sizeof(shExInfo);
		shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		shExInfo.hwnd = nullptr;
		shExInfo.lpFile = _T(programm.c_str());
		shExInfo.lpParameters = _T(programmParams.c_str());
		shExInfo.lpDirectory = nullptr;
		shExInfo.hInstApp = nullptr;

		if (ShellExecuteEx(&shExInfo))
		{
			WaitForSingleObject(shExInfo.hProcess, INFINITE);
			CloseHandle(shExInfo.hProcess);

			auto cb = Local<Function>::Cast(args[3]);
			cb->Call(Null(isolate), 0, {});
		}
	}

	void Init(Local<Object> exports, Local<Object> module) {
		auto isolate = Isolate::GetCurrent();

		exports->Set(String::NewFromUtf8(isolate, "run"),
			FunctionTemplate::New(isolate, Run)->GetFunction());
	}

	NODE_MODULE(runas, Init)
}
