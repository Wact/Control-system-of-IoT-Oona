(function() {
'use strict';
    // IP of your controller
    var ip = "http://192.168.0.98:7887";
    var safety = false;
    var homeplan = true;
    let components = new Map();
    
    angular
        .module('Homepage', ['ngCookies'])
        .controller('HomepageController', HomepageController)
        .controller('GeneralController', GeneralController)
        .controller('SafetyController', SafetyController)
        .controller('LedsController', LedsController);

    HomepageController.$inject = ['$scope', '$interval', '$http', '$cookies'];
    function HomepageController ($scope, $interval, $http, $cookies) {
        $scope.setZeroCookie = () => {
            $cookies.remove('status');
        }
        $interval(() => {
            $scope.homeplanShow = homeplan;
            $scope.safetyShow = safety;
            $scope.status = $cookies.get('status');
            if($scope.status == "adult") { $scope.adult = true; } else { $scope.adult = false; }
            $http({method: 'GET',
                url: 'getData.php',
                cache: false
            })
            .then(function(response) {
                var arr = angular.fromJson(response.data);
                angular.forEach(arr, function(value, key) {
                    components.set(key, value);
                }, this);
            });
            function draw() {
                var canvas = document.getElementById('canvasHome');
                var cx = canvas.getContext('2d');
                var lampOFF = document.createElement("img");
                var lampON = document.createElement("img");
                lampON.src = "items/lampON.png";
                lampOFF.src = "items/lampOFFDark.png";
                function wall(bx, by, ex, ey){                   /* Function of drawing the wall */
                    cx.beginPath();
                    cx.moveTo(bx, by);
                    cx.lineTo(ex, ey);
                    cx.closePath();
                    cx.stroke();
                }

                function drawWalls(){                            /* Plan of your home */
                    // External walls
                    wall(2, 2, 30, 2);
                    wall(190, 2, 220, 2);
                    wall(220, 2, 265, 2);
                    wall(455, 2, 500, 2);
                    wall(500, 2, 530, 2);
                    wall(718, 2, 748, 2);
                    wall(748, 2, 748, 270);
                    wall(748, 330, 748, 498);
                    wall(748, 498, 2, 498);
                    wall(2, 498, 2, 2);
                            /* Interroom walls */
                    // Main room
                    wall(220, 2, 220, 250);
                    wall(220, 250, 310, 250);
                    wall(390, 250, 500, 250)
                    wall(500, 250, 500, 2);

                    // Kitchen
                    wall(500, 250, 600, 250);
                    wall(660, 250, 748, 250);

                    // Northern bedroom
                    wall(2, 250, 80, 250);
                    wall(140, 250, 220, 250);

                    // Southern bedroom
                    wall(2, 350, 100, 350);
                    wall(160, 350, 260, 350);
                    wall(260, 350, 260, 498);

                    // Lavatory
                    wall(260, 350, 270, 350);
                    wall(330, 350, 340, 350);
                    wall(340, 350, 340, 498);

                    // Bathroom
                    wall(340, 350, 360, 350);
                    wall(420, 350, 440, 350);
                    wall(440, 350, 440, 498);
                }

                function lighting() {                        /* Lighting */
                    if(components.get('led1') === '2'){							/* Southern bedroom */
                        //lampON.onload = () => {
                        cx.drawImage(lampON, 50, 50, 100, 100);
                        //};
                    } else {
                        //lampOFF.onload = () => {
                        cx.drawImage(lampOFF, 50, 50, 100, 100);
                        // cx.fillStyle("#FFEFDB");
                        // cx.beginPath();
                        // cx.arc(100,75,50,0,2*Math.PI);
                        // cx.stroke();
                        //};
                    }		
                    // console.log(components.get("led1"));
                    // if(lamp1 == 2){							/* Main room */
                    //     cx.drawImage(lampON, 300, 50, 100, 100);
                    // } else {
                    //     cx.drawImage(lampOFF, 300, 50, 100, 100);
                    // }				
                    // if(lamp2 == 2){							/* Kitchen */
                    //     cx.drawImage(lampON, 580, 50, 100, 100);
                    // } else {
                    //     cx.drawImage(lampOFF, 580, 50, 100, 100);
                    // }
                    // if(lamp3 == 2){							/* Southern bedroom */
                    //     cx.drawImage(lampON, 80, 370, 100, 100);
                    // } else {
                    //     cx.drawImage(lampOFF, 80, 370, 100, 100);
                    // }
                    // if(lamp4 == 2){							/* Lavatory */
                    //     cx.drawImage(lampON, 250, 370, 100, 100);
                    // } else {
                    //     cx.drawImage(lampOFF, 250, 370, 100, 100);
                    // }
                    // if(lamp5 == 2){							/* Bathroom */
                    //     cx.drawImage(lampON, 340, 370, 100, 100);
                    // } else {
                    //     cx.drawImage(lampOFF, 340, 370, 100, 100);
                    // }
                    // if(lamp6 == 2){							/* Hall 1 */
                    //     cx.drawImage(lampON, 50, 240, 100, 100);
                    // } else {
                    //     cx.drawImage(lampOFF, 50, 240, 100, 100);
                    // }
                    // if(lamp7 == 2){							/* Hall 2 */
                    //     cx.drawImage(lampON, 550, 330, 100, 100);
                    // } else {
                    //     cx.drawImage(lampOFF, 550, 330, 100, 100);
                    // }
                }

                drawWalls();
                lighting();
            }
            draw();
        }, 20);
    }

    GeneralController.$inject = ['$scope'];
    function GeneralController ($scope) {
        $scope.setPlace = () => {
            homeplan = true;
            safety = false;
        }
    }

    SafetyController.$inject = ['$scope'];
    function SafetyController ($scope) {
        $scope.setPlace = () => {
            homeplan = false;
            safety = true;
        }
    }

    LedsController.$inject = ['$scope','$http','$interval'];
    function LedsController ($scope, $http, $interval) {
        $scope.change = () => {
            var led1Stat;
            components.get('led1')==='2' ? led1Stat=1 : led1Stat=2;
            $http({
                method: 'POST',
                url: ip,
                cache: false,
                params: { led1: led1Stat.toString() }
            });
        }
    }
})();