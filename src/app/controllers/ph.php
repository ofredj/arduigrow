<?php
require_once("base.php");

class PHHandler extends Base_Controller
{
	function get() {
		$arduigrow = new ArduiGrow_Model;
		/* render view  */
		$this->render('PH', 
			array('ph' => $arduigrow->series('ph'))); 
	}
}
