

<center>
<div id="block" class="block">
<ul >
<li><a href="webcam"><b>&lt;</b></a></li>
<li>
  <!-- Three columns of text below the carousel -->
<div class="container marketing">
      <div class="row">
        <div class="col-lg-4">
	<a href="light">
          <img class="img" src="<?echo ($data['light'] == "true")?"img/sun.png":"img/night.png";?>" alt="Generic placeholder image" style="width: 64px; height: 64px;">
          <h4>Light: <?echo ($data['light'] == "true")?"On":"Off";?></h4>
	</a>
        </div><!-- /.col-lg-4 -->
        <div class="col-lg-4">
	<a href="temperature">
          <img class="img" src="img/thermometer.png" alt="Generic placeholder image" style="width: 64px; height: 64px;">
          <h4>Temp: <?echo $data['temperature'];?>C</h4>
	</a>
        </div><!-- /.col-lg-4 -->
        <div class="col-lg-4">
	<a href="humidity">
          <img class="img" src="img/humidity.png" alt="Generic placeholder image" style="width: 64px; height: 64px;">
          <h4>Humidity: <?echo $data['humidity'];?>%</h4>
	</a>
        </div><!-- /.col-lg-4 -->
      </div><!-- /.row -->
      <div class="row">
        <div class="col-lg-4">
	<a href="ph">
          <img class="img" src="img/PH.png" alt="Generic placeholder image" style="width: 64px; height: 64px;">
          <h4>pH: <?echo $data['ph'];?></h4>
	</a>
        </div><!-- /.col-lg-4 -->
        <div class="col-lg-4">
	<a href="ec">
          <img class="img" src="img/EC.png" alt="Generic placeholder image" style="width: 64px; height: 64px;">
          <h4>EC: 3.8</h4>
	</a>
        </div><!-- /.col-lg-4 -->
        <div class="col-lg-4">
	<a href="webcam">
          <img class="img" src="img/video.png" alt="Generic placeholder image" style="width: 64px; height: 64px;">
          <h4>Webcam</h4>
	</a>
        </div><!-- /.col-lg-4 -->
      </div><!-- /.row -->
<!--
      <div class="row">
        <div class="col-lg-4">
          <img class="img" src="img/water.png" alt="Generic placeholder image" style="width: 64px; height: 64px;">
          <h4>Water Tank: 64%</h4>
        </div>
        <div class="col-lg-4">
          <img class="img" src="img/fertilizer.png" alt="Generic placeholder image" style="width: 64px; height: 64px;">
          <h4>Fertilizer Tank: 55%</h4>
        </div>
       <div class="col-lg-4">
          <img class="img" src="img/PH-down.png" alt="Generic placeholder image" style="width: 64px; height: 64px;">
          <h4>PH Down Tank: 60%</h4>
        </div>
      </div>
-->
</div>

</li>

<li><a href="light"><b>&gt;</b></a></li>
<ul>
</div>
</center>
<!--
<div class="container data">
          <img class="img-circle" src="data/00000001.png" alt="ScreenShot">
</div>
-->
