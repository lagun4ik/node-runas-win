import gulp from "gulp";
import shell from "gulp-shell";
import rimraf from "rimraf";
import run from "run-sequence";
import watch from "gulp-watch";
import gulpServer from "gulp-live-server";

let server;
const paths = {
    js: ['./src/**/*.js'],
    destination: './dist',
    src: './src/**/*',
    ext: './src/**/*.cpp'
};

gulp.task('default', cb => run('server', 'build', ['watchJs', 'watchExt'], cb));

gulp.task('build', cb => run('clean', 'babel', 'restart', cb));

gulp.task('buildExt', cb => run('gyp', 'restart', cb));

gulp.task('clean', cb => rimraf(paths.destination, cb));

gulp.task('babel', shell.task(['babel src --out-dir dist']));

gulp.task('gyp', shell.task(['node-gyp --python "C:\Program Files\Python27\python.exe" build']));

gulp.task('server', () => server = gulpServer.new(paths.destination));

gulp.task('restart', () => gulpServer.start.bind(server)());

gulp.task('watchJs', () => watch(paths.js, () => gulp.start('build')));

gulp.task('watchExt', () => watch(paths.ext, () => gulp.start('buildExt')));
