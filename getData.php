<?php header('Content-Type: text/html; charset=UTF-8;');
    header('Access-Control-Allow-Origin: *'); 
    header("Cache-Control: no-cache, must-revalidate");  
    header("Pragma: no-cache"); 
?>

<?php

    $connection = mysqli_connect("localhost", "root", "", "smart_house");
    mysqli_set_charset($connection, "utf8");

    $result = mysqli_query($connection, " SELECT * FROM data");

    while($row = mysqli_fetch_array($result)) {
        $arr[$row['data']] = $row['value'];
    }

    echo json_encode($arr);
?>