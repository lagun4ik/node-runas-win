{
  "targets": [
    {
      "target_name": "runas-win",
      "sources": [ "./src/runas-win.cpp" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}