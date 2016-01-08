var exec = require("child_process").exec;
var fs = require("fs");
var flag = process.argv[2];

function main(){

    //Try using pkg-config, but if it fails and it is on Windows, try the fallback
    exec("pkg-config opencv " + flag, function(error, stdout, stderr){
        if(error){
            if(process.platform === "win32"){
                fallback();
            }
        }
        else{
            console.log(stdout);
        }
    });
}

//======================Windows Specific=======================================

function fallback(){
    exec("echo %OPENCV_DIR%", function(error, stdout, stderr){
        stdout = cleanupEchoOutput(stdout);
        if(error){
            console.log("ERROR: There was an error reading OPENCV_DIR");
        }
        else if(stdout === "%OPENCV_DIR%") {
            console.log("ERROR: OPENCV_DIR doesn't seem to be defined");
        }
        else {
            getVisualStudioVersion(function(version){
                getProcessArch(function(bits){
                    printPaths(stdout, version, bits);
                });
            });
        }
    });
}

function printPaths(opencvPath, version, bits){
    if(flag === "--cflags") {
        console.log("\"" + opencvPath + "include\"");
        console.log("\"" + opencvPath + "include\\opencv\"");
    }
    else if(flag === "--libs") {
        var libPath = opencvPath + bits + "\\vc" + version + "\\lib\\";

        fs.readdir(libPath, function(err, files){
            if(err){
                console.log("ERROR: couldn't read the lib directory");
                console.log(err);
            }

            var libs = "";
            for(var i = 0; i < files.length; i++){
                if(getExtension(files[i]) === "lib"){
                    libs = libs + " \"" + libPath + files[i] + "\" \r\n ";
                }
            }
            console.log(libs);
        });
    }
    else {
        console.log("Error: unknown argument '" + flag + "'");
    }
}

//This gets the architecture of the NodeJS that is running this script,
//either x86 or x64
function getProcessArch(cb){
    exec("echo %PROCESSOR_ARCHITECTURE%", function(error, stdout, stderr) {
        if(!error) {
            var arch = cleanupEchoOutput(stdout);
            if(arch === "AMD64"){
                cb("x64");
            }
            else if(arch === "x86"){
                cb("x86");
            }
            else {
                console.log("ERROR: Unrecognized architecture");
            }
        }
        else {
            console.log("ERROR: There was an error getting the architecture");
        }
    });
}

function getVisualStudioVersion(cb, version){
    if(typeof(version) === "undefined") version = 11;
    exec("reg query HKEY_CLASSES_ROOT\\VisualStudio.DTE." + version + ".0",
        function(error, stdout, stderr){
            if(!error){
                cb(version);
            }
            else if(version < 13) {
                //Try the next version
                getVisualStudioVersion(cb, version + 1);
            }
            else {
                console.log("ERROR: Can't find Visual Studio");
            }
        });
}

function cleanupEchoOutput(s){
    return s.slice(0, s.length - 2);
}

function getExtension(s){
    return s.substr(s.lastIndexOf(".") + 1);
}
main();
