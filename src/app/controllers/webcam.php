<?php
require_once("base.php");

class WebcamHandler extends Base_Controller
{
	function get() {
		$this->css("//vjs.zencdn.net/4.6/video-js.css");
		$this->js("//vjs.zencdn.net/4.6/video.js");
		$this->render('webcam');
	}
}
