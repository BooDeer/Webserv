<?php

    session_start();
    $_SESSION["hola"] = "NICE HADCHI KHADAM";
    $value = 'something from somewhere';

    setcookie("test", $value);
?>