<?php
  $log = 'log/acc.txt';
  $ip = getenv("REMOTE_ADDR");
  $timestamp = date("m/d/y H:i",time());
  $url = 'index.html';

  $fp = @fopen($log, "a+") or die($log.'error');
  $newline = "[$timestamp][$ip][$url]\r\n";
  fwrite($fp,$newline);
  fclose($fp);

  $log = 'log/count.txt';
  $fp = @fopen($log, "r+") or die($log.'error');
  $count = fgets($fp, 64);
  flock($fp, 2);
  $count = (int)$count + 1;
  rewind($fp);
  fputs($fp, $count);
  fclose($fp);
?>