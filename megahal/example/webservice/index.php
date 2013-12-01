<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>MegaHAL Chat</title>
<script src="http://code.jquery.com/jquery-1.9.1.min.js"></script>
<!-- Latest compiled and minified CSS -->
<link rel="stylesheet" href="//netdna.bootstrapcdn.com/bootstrap/3.0.2/css/bootstrap.min.css">
<!-- Optional theme -->
<link rel="stylesheet" href="//netdna.bootstrapcdn.com/bootstrap/3.0.2/css/bootstrap-theme.min.css">
<!-- Latest compiled and minified JavaScript -->
<script src="//netdna.bootstrapcdn.com/bootstrap/3.0.2/js/bootstrap.min.js"></script>
<script src="megahal-chat.js"></script>
<link type="text/css" rel="stylesheet" href="style.css" />
</head>

<div class="megahal-wrapper">
    <div class="megahal-menu">
        <p class="megahal-welcome">Welcome to MegaHAL Chat!<b></b></p>
        <div style="clear:both"></div>
    </div>

    <div class="megahal-chatbox"></div>

    <form class="form-inline input-form" role="form">
        <div class="form-group" style="width: 75%;">
            <input class="form-control user-input" id="focused-input" type="text" size="255" placeholder="Say something"/>
        </div>
        <button class="btn btn-default submit-btn">Send</button>
    </form>
</div>


<hr border=0/>
<i>Copyright (C) 2013 Misha Nasledov</i>
</body>
</html>
