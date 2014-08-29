<?php
require_once("base.php");

class LightHandler extends Base_Controller
{
	function get() {
		$arduigrow = new ArduiGrow_Model;
		/* render view  */
		$this->render('light', 
			array('light' => $arduigrow->series('light'))); 
	}
}
