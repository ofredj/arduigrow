<?php
require_once("base.php");

class PumpHandler extends Base_Controller
{
	function get() {
		$arduigrow = new ArduiGrow_Model;
		$arduigrow->series();
		/* render view  */
		$this->render('EC', 
			array('ec' => $arduigrow->series('ec'))); 
	}
}
