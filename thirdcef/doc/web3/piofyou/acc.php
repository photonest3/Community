<?php
  $log = 'log/down.txt';
  $ip = getenv("REMOTE_ADDR");
  $timestamp = date("m/d/y H:i",time());
   
$id = $_GET['id'];

    $url = 'download';

  $fp = @fopen($log, "a+") or die($log.'error');
  $newline = "[$timestamp][$ip][$url]\r\n";
  fwrite($fp,$newline);
  fclose($fp);

?>