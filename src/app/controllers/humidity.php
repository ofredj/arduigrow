<?php
require_once("base.php");

class HumidityHandler extends Base_Controller
{
	function get() {
		$arduigrow = new ArduiGrow_Model;
		/* render view  */
		$this->render('humidity', 
			array('humidity' => $arduigrow->series('humidity'))); 
	}
}
