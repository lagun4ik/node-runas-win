# Runas-win

Promise for run command with administrator privilege and hide mode.


## Installing

```sh
npm install runas-win
```

## Use

```js
import runasPromise from 'runas-win'

runasPromise('node', '-v', {hide:false, admin:true})
    .then(()=>console.log('finish'));
```


### runasPromise(command, args[, options])

* `options` Object
  * `hide` Boolean - Hide the console window, `true` by default.
  * `admin` Boolean - Run command as administrator, `false` by default.