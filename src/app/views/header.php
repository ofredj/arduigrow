<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="refresh" content="300">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Arduigrow</title>
    <link rel="icon" href="img/favicon.ico" />
    <script type="text/javascript" src="js/jquery.js"></script>

    <!-- Bootstrap -->
    <?php foreach (array_values($data['css_files']) as $css) : ?>
    <link href="<?php echo $css;?>" rel="stylesheet">
    <?php endforeach ?>



  </head>
  <body>
    

<nav class="navbar navbar-default" role="navigation">
  <div class="container-fluid">
    <!-- Brand and toggle get grouped for better mobile display -->
      <div class="navbar-header">
      <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
        <span class="sr-only">Toggle navigation</span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
        <span class="icon-bar"></span>
      </button>
	  <a href="home">
          <img class="img" src="img/arduigrow-white.png" alt="Generic placeholder image" style="width:256px; height: 64px;">
	</a>
    </div>

    <!-- Collect the nav links, forms, and other content for toggling -->
    <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
      <ul class="nav navbar-nav navbar-right">
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown">Menu<span class="caret"></span></a>
          <ul class="dropdown-menu" role="menu">
            <li><a href="/temperature">Temperatures</a></li>
            <li><a href="/humidity">Humidity</a></li>
            <li class="divider"></li>
            <li><a href="/ph">PH and EC</a></li>
            <li><a href="/ec">PH and EC</a></li>
            <li class="divider"></li>
            <li><a href="/water-level">Water Tanks Level</a></li>
            <li class="divider"></li>
            <li><a href="/stream">Live Stream</a></li>
            <li><a href="/movie">Grow Movie</a></li>
            <li class="divider"></li>
            <li><a href="#">Configure</a></li>
          </ul>
        </li>
        <li><a href="#">ArduiGrow</a></li>
      </ul>
    </div><!-- /.navbar-collapse -->
  </div><!-- /.container-fluid -->
</nav>
