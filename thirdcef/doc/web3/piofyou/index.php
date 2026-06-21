<!DOCTYPE html>
<html lang="zxx">

<head>
    <title>The ultimate photo gallery software,designed for home and individuals,supports video,support encrypted storage.</title>
    <meta name="keywords" content="enterprise instant messaging,instant messaging software,community forum software,bulletin board software,bbs software,instant messaging system,web messenger server,instant messaging solutions,enterprise messenger,office instant messaging,lan messenger" />
    <meta name="description" content="The ultimate photo gallery software,designed for home and individuals,supports video,support encrypted storage." />
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="language" content="en" />
    <meta name="author" content="Stormiweb" />
    <link href="/favicon.ico" rel="icon" />
    <meta charset="utf-8" />
    <script>
        addEventListener("load", function() {
            setTimeout(hideURLbar, 0);
        }, false);

        function hideURLbar() {
            window.scrollTo(0, 1);
        }

    </script>
    <!-- Custom Theme files -->
    <link href="css/bootstrap.css" type="text/css" rel="stylesheet" media="all">
    <link href="css/style.css" type="text/css" rel="stylesheet" media="all">
    <!-- font-awesome icons -->
    <link href="css/font-awesome.min.css" rel="stylesheet">
    <!-- //Custom Theme files -->
    <!-- online-fonts -->
    <!--<link href="http://fonts.googleapis.com/css?family=Raleway:100,100i,200,200i,300,300i,400,400i,500,500i,600,600i,700,700i,800" rel="stylesheet">-->
    <!-- //online-fonts -->


    <style>




.layer {
    /*padding: 8em 0 1px;*/
    background-image: -webkit-radial-gradient(50% 100%,circle farthest-corner,#273716,#000);
    background-image: radial-gradient(circle farthest-corner at 50% 100%,#273716,#000);
}


.index-intro-text h1 {
    margin: 0 0 10px!important;
    line-height: 120%;
    color: #fff;
    font-size: 2.45em;
       margin-bottom:30px;
}
.index-intro-text h2 {
    color: #ccd3d6;
   
       margin-bottom:10px;
    font-size: 1.5625em;
    font-weight: 200;
}
    </style>
</head>

<body>
<script type="text/javascript"> 
function test() {   

  var _parentWin = window.parent  

_parentWin.contactform.email.value = ""; 
_parentWin.contactform.issues.value = ""; 

 

var obj= _parentWin.document.getElementById("lblmsg"); 
 if(obj){
 obj.innerHTML="Thanks, we'll respond as soon as we can.";  
 }
}
 </script>
 


 
    欢迎 <?php echo $_POST["email"]; ?>!<br>

    你的年龄是 <?php echo $_POST["issues"]; ?>  岁。
<?php	
	$issues = $_POST['issues'];
			$email = $_POST['email'];
		

			$ip = getenv("REMOTE_ADDR");
			$timestamp = date("m/d/y H:i",time());
			
			$issues = str_replace("　"," ",$issues);
			$issues = trim($issues);
 $log = 'log/log.txt';			

  $fp = @fopen($log, "a+") or die($log.'error');
  $newline = "[$timestamp][$ip][$email][$issues]\r\n";
  fwrite($fp,$newline);
  fclose($fp);
		

echo "<script type='text/javascript'>test();</script>";
		
	?>	

</body>

</html>
