<?php header('Content-Type: text/html; charset=UTF-8;');
    header('Access-Control-Allow-Origin: *'); 
    header("Cache-Control: no-cache, must-revalidate");  
    header("Pragma: no-cache"); 
?>

<html ng-app="Homepage">
	<head>
		<meta charset="utf-8">
		<title>Уна</title>
		<link href="main.css" rel="stylesheet">
        <script src="angular.min.js"></script>
		<script src="angular-cookies.js"></script>
		<script src="homepage.js"></script>
	</head>
	<body class="container" ng-controller="HomepageController">
		<?php
			if(isset($_POST['submit'])){
				$connection =  mysqli_connect("localhost", "root", "", "smart_house");
				mysqli_set_charset($connection, "utf8");

				$result = mysqli_query($connection, " SELECT * FROM users WHERE user_login ='".mysqli_real_escape_string($connection,$_POST['login'])."' LIMIT 1");

				$row = mysqli_fetch_array($result);
				if($row['user_password'] === hash('sha256',$_POST['password'])) { 
					setcookie("status", $row['user_status'], time()+3600*24*30, "/"); ?>
					<table id="hat"><tr>
					<td><div ng-controller="GeneralController"><button id="general" type="button" ng-click="setPlace();">Общая Информация</button></div></td>
					<td><div ng-controller="SafetyController"><button id="safety" type="button" ng-click="setPlace();">Безопасность</button></div></td>
					<td><div id="author">Автор проекта - Малыгин Артем. The author of the project is Malygin Artyom.</div></td>
					<td><form name="btnExit" action="index.php" method="post"><input name="submit" type="submit" value="Выйти" ng-click="setZeroCookie();"/>
						<input value="" name="login" size="40" type="hidden" />
						<input value="" name="password" size="40" type="hidden" />
					</form></td></br>
					</tr></table>
					<!--<p text-align="center"><form name="btnExit" action="index.php" method="post"><input name="submit" type="submit" value="Выйти" />
						<input value="" name="login" size="40" type="hidden" />
						<input value="" name="password" size="40" type="hidden" />
					</form></p>-->
					<div id="content" ng-view="content">
						<p id="greeting">Привет, <?php echo $_POST['login']; ?></p>
						<p>Your status: {{status}}</p>
						<div ng-if="homeplanShow">
							<canvas id="canvasHome" width="750px" height="500px"></canvas>
							</br>
							<div ng-controller="LedsController">
								<!--<label ng-if="isActive==1"><input type="checkbox" ng-model="led1off" ng-change="change(2);" ng-checked="false">LED 1</label>
								<label ng-if="isActive==2"><input type="checkbox" ng-model="led1on" ng-change="change(1);" ng-checked="true">LED 1 CHECKED</label>-->
								<button ng-if="adult" type="button" ng-click="change();">LED 1</button>
							</div>
						</div>
						<div ng-if="safetyShow">
							Здесь будет информация о безопасности в вашем доме
						</div>
					</div>
				<?php } 
				else { ?>
				<section id="authorization">
					</br>
					<p>Авторизация</p>
					<!--<p class="error">Были введены неправильно логин и/или пароль</p>-->
					<form name="authorization" action="index.php" method="post">
						<input value="" name="login" size="40" type="text" placeholder="Логин" required>
						<br>
						<br>
						<input value="" name="password" size="40" type="password" placeholder="Пароль" required>
						<br>
						<br>
						<input name="submit" type="submit" value="Войти">
					</form>
					<form name="btnGuest" action="index.php" method="post">
						<input value="guest" name="login" size="40" type="hidden" />
						<input value="guest" name="password" size="40" type="hidden" />
						<input name="submit" type="submit" value="Войти гостем"/>
					</form>
				</section>
				<?php } ?>
			<?php } 
			else { ?>
			<section id="authorization">
				</br>
				<p>Авторизация</p>
				<form name="authorization" action="index.php" method="post">
					<input value="" name="login" size="40" type="text" placeholder="Логин" required>
					<br>
					<br>
					<input value="" name="password" size="40" type="password" placeholder="Пароль" required>
					<br>
					<br>
					<input name="submit" type="submit" value="Войти" />
				</form>
				<form name="btnGuest" action="index.php" method="post">
					<input value="guest" name="login" size="40" type="hidden" />
					<input value="guest" name="password" size="40" type="hidden" />
					<input name="submit" type="submit" value="Войти гостем"/>
				</form>
			</section>
			<?php } ?>
	</body>
</html>