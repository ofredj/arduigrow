<?php
require_once("../app/lib/Toro.php");
require_once("../app/controllers/home.php");
require_once("../app/controllers/light.php");
require_once("../app/controllers/temperature.php");
require_once("../app/controllers/humidity.php");
require_once("../app/controllers/stream.php");
require_once("../app/controllers/webcam.php");
require_once("../app/controllers/ph.php");
require_once("../app/controllers/ec.php");

Toro::serve(array(
    "/" => "HomeHandler",
    "/home" => "HomeHandler",
    "/light" => "LightHandler",
    "/temperature" => "TemperatureHandler",
    "/humidity" => "HumidityHandler",
    "/ph" => "PHHandler",
    "/ec" => "ECHandler",
    "/stream" => "StreamHandler",
    "/webcam" => "WebcamHandler",
    "/movie" => "MovieHandler",
));

?>
