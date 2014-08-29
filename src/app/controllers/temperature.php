<?php
require_once("base.php");

class TemperatureHandler extends Base_Controller
{
	function get() {
		$arduigrow = new ArduiGrow_Model;
		/* render view  */
		$this->render('temperature', 
			array('temperature' => $arduigrow->series('temperature'))); 
	}
}
