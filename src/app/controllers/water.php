<?php
require_once("base.php");

class WaterHandler extends Base_Controller
{
	function get() {
		$arduigrow = new ArduiGrow_Model;
		/* render view  */
		$this->render('water', 
			array('water' => $arduigrow->series('water'))); 
	}
}
