<?php
// Log the user out
if (isset($_GET['logout'])){
    setcookie ("user", null, time() - 3600,'/');
    header('Location: authenticate',403);
}

if (isset($_POST['user']) && isset($_POST['password'])){
    // Validate that the username and password match
    if ($_POST['user'] == 'admin' && MD5($_POST['password']) == '5f4dcc3b5aa765d61d8327deb882cf99'){
        // Authentication test passed
        setcookie('user',$_POST['user'],0,'/');
        header('Location: home',301);
    } else {
        header('Location: authenticate',403);
    }
} else {
    if(!($this->p == 'authenticate')){
        if (isset($_COOKIE['user'])){
        // All is well, the admin cookie is set.
        } else {
            header('Location: authenticate',403);
        }
    }
}


?>
