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
<link type="text/css" rel="stylesheet" href="style.css" />
</head>

<div id="wrapper">
    <div id="menu">
        <p class="welcome">Welcome to MegaHAL Chat!<b></b></p>
        <div style="clear:both"></div>
    </div>

    <div id="chatbox" class="chatbox"></div>

    <form class="form-inline input-form" role="form">
        <div class="form-group" style="width: 75%;">
            <input class="form-control user-input" id="focused-input" type="text" size="255" placeholder="Say something"/>
        </div>
        <button class="btn btn-default submit-btn">Send</button>
    </form>
</div>

<script type="text/javascript">
     function getHALText(text) {
         return '<font color="red">HAL: </font>'+text+'<br/><br/>';
     }

     function getUserPrefix() {
         return '<font color="blue">User: </font>';
     }

    // jQuery Document
    $(document).ready(function() {
        // get greeting
        $.get('/~misha/megahal/talk.php', {}, function(res) {
            if (res) {
                var resObj = $.parseJSON(res);
                $('.chatbox').html(getHALText(resObj.reply));
            }
        });

        $('.input-form').submit(function(e) {
            var userMsgBox = $('.user-input');
            var clientMsg = userMsgBox.val();
            var chatbox = $('.chatbox');

            chatbox.append(getUserPrefix(''));
            $(document.createTextNode(clientMsg)).appendTo(chatbox);
            chatbox.append('<br/><br/>');
            userMsgBox.val('');
            $.get('/~misha/megahal/talk.php?q='+encodeURIComponent(clientMsg), {}, function(res) {
                if (res) {
                    var resObj = $.parseJSON(res);
                    chatbox.append(getHALText(resObj.reply));
                }
                chatbox.animate({
                      scrollTop: chatbox.height()
                }, 300);
            });

            return false;
        });
    });
</script>
<hr border=0/>
<i>Copyright (C) 2013 Misha Nasledov</i>
</body>
</html>
