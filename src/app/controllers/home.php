<?php
require_once("base.php");
require_once("../app/models/arduigrow.php");

class HomeHandler extends Base_Controller
{
	function get() {
		$arduigrow = new ArduiGrow_Model;
		$data = $arduigrow->last();
		$this->render('home', $data);
	}
}
