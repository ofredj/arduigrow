<?php

class Base_Controller
{
	var $css_files = array(  0 => 'css/bootstrap.min.css',
				 1 => 'css/style.css');
	var $js_files = array( 
		0 => 'js/bootstrap.min.js');

	function js ($url) {
		array_push($this->js_files, $url);
	}
	
	function css ($url) {
		array_push($this->css_files, $url);
	}

	function view($view = NULL, $data = array())
	{
		$view_file = "../app/views/". $view . ".php";
		if (file_exists($view_file)){
			include($view_file);
		}
	}
	
	function header()
	{
		$data = array ( 'css_files' => $this->css_files );
		$this->view('header', $data);
	}
	
	function footer($data)
	{
		$data['js_files'] = $this->js_files;
		$this->view('footer', $data);
	}


	function render($view=NULL, $data=array())
	{
		$this->header();
		$this->view($view, $data);
		$this->footer($data);
	}
}
