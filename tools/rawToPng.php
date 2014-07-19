<?php

$im = ImageCreateTrueColor(256, 256);

$x = 0;
$y = 0;
$r = $g = $b = $a = 0;
$i = 0; 

$data = file_get_contents("texture.raw");

$i = 0;

while ($i < strlen($data)) {
	$r = ord($data{$i++});
	$g = ord($data{$i++});
	$b = ord($data{$i++});
	$a = ord($data{$i++});

	$color = $b + ($g << 8) + ($r << 16);
	ImageSetPixel($im, $x, $y, $color);

	$x++;
	if ($x >= 256) {
		$x = 0;
		$y++;
	}
}

// show texture margins
ImageRectangle($im, 0, 0, 255, 255, 0xffffff);

ImagePng($im, "texture.png");
