<?php

function switchto_site(){
    // Run this before opening content or running scandirs on the site that is being administered.
    $directoryroot = e::_dirup(dirname($_SERVER['SCRIPT_FILENAME'])) . '/';    
    $return = chdir($directoryroot);
    return $return;
}

function switchto_admin(){
    // Run this when you are done doing whatever you were doing when you ran switchto_site ;-)
    $directoryroot = e::_dirup(dirname($_SERVER['SCRIPT_FILENAME'])) . '/_admin';    
    $return = chdir($directoryroot);
    return $return;
}

function text_to_path($name,$timestamp = FALSE){
    $allowedchars = 'abcdefghijklmnopqrstuvwxyz-0123456789';
    $path = '';
    $name = strtolower($name);
    $name = str_split($name);
    foreach($name as $char){
        if (strstr($allowedchars,$char)){
            $path .= $char;
        } else {
            $path .= '_';
        }
    }
    while(strstr($path,'__')){
        $path = str_ireplace('__', '_', $path);
    }
    if ($timestamp){
        $path = date('YmdHi') . '.' . $path;
    }
    
    return $path;
}

?>
