<?php

class ArduiGrow_Model 
{
	var $data = array();
	var $csv_file = '../data/arduigrow.csv';

	private function read_csv_data()
	{
		$data = array( 
			'humidity' => array(), 
			'temperature' => array(),
			'light' => array());
		$lines = explode("\n",file_get_contents($this->csv_file));
		foreach ($lines as $line){
			$columns = explode(',', $line);
			if (empty($columns) || count($columns) <= 2) continue;
			$light = ($columns[3] == 'true') ? 1 : 0 ;
			$pH = empty($columns[4]) ? null : $columns[4] ;
			$EC = empty($columns[5]) ? null : $columns[5] ;
			$EC = rand(1,6) ;
			$pH = rand(4,7) ;
			/* upadte last values */
			$data['last'] = array( 
				'date' => date('r', $columns[0]),
				'temperature' => $columns[2],
				'humidity' => $columns[1], 
				'light' => $columns[3] );
			/* add data to metric series */
			$timestamp =  $columns[0] * 1000 ;
			$data['humidity'][] = array( $timestamp,  (float)$columns[1]);
			$data['temperature'][] = array( $timestamp,  (float)($columns[2]));
			$data['light'][] = array( $timestamp,  $light);
			if ($pH) {
				$data['ph'][] = array( $timestamp, (float) $pH);
				$data['last']['ph'] = $pH;  
			}
			if ($EC) {
				$data['ec'][] = array( $timestamp, (float) $EC);
				$data['last']['ec'] = $EC;  
			}
		}
		return $data;
	}

	public function last_temperature()
	{
		if (empty($this->data)) {
			$this->data = $this->read_csv_data();	
		}
		return $this->data['last']['temperature'];
	}

	public function last_humidity()
	{
		if (empty($this->data)) {
			$this->data = $this->read_csv_data();	
		}
		return $this->data['last']['humidity'];
	}

	public function last_light()
	{
		if (empty($this->data)) {
			$this->data = $this->read_csv_data();	
		}
		return $this->data['last']['light'];
	}

	public function last()
	{
		if (empty($this->data)) {
			$this->data = $this->read_csv_data();	
		}
		return $this->data['last'];
	}
	
	public function series($metric = "temperature")
	{
		if (empty($this->data)) {
			$this->data = $this->read_csv_data();	
		}
		return json_encode($this->data[$metric]);
	}

	public function amcharts()
	{
		if (empty($this->data)) {
			$this->data = $this->read_csv_data();	
		}
		return $this->data['amstring'];
	}
}
