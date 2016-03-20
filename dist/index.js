'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});

exports.default = (command, args, options) => new Promise((resolve, reject) => {
    if (args == null) {
        args = [];
    }
    if (options == null) {
        options = {};
    }
    if (options.hide == null) {
        options.hide = true;
    }
    if (options.admin == null) {
        options.admin = false;
    }
    if (command == null) {
        reject('not transferred to the command to carry out');
    }

    require('../build/Release/runas-wind.node').run(command, args, options, () => resolve());
});