	<div id="footer">
		<p>Last updated on <?php if (isset($data['date'])) echo $data['date'];?>.</p>
	</div>

	<!-- include all javascript at the end -->
    	<?php foreach (array_values($data['js_files']) as $js) : ?>
    	<script src="<?php echo $js;?>"></script>
    	<?php endforeach ?>
  </body>
</html>
