<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>MegaHAL Chat</title>
<link type="text/css" rel="stylesheet" href="style.css" />
</head>

<div id="wrapper">
    <div id="menu">
        <p class="welcome">Welcome<b></b></p>
        <div style="clear:both"></div>
    </div>

    <div id="chatbox"></div>

        <input name="usermsg" type="text" id="usermsg" size="63" />
        <input name="submitmsg" type="button"  id="submitmsg" value="Send" />
</div>

<script src="http://code.jquery.com/jquery-1.9.1.min.js"></script>

<script type="text/javascript">
    // jQuery Document
    $(document).ready(function() {
        // get greeting
        $.get('/~misha/megahal/talk.php', {}, function(res) {
            var resObj = $.parseJSON(res);
            $('#chatbox').html('HAL: '+resObj.reply+'<br/><br/>');
        });
    });

    $('#submitmsg').click(function() {
        var userMsgBox = $('#usermsg');
        var clientMsg = userMsgBox.val();
        var chatbox = $('#chatbox');
        $(document.createTextNode('User: '+clientMsg)).appendTo(chatbox);
        chatbox.append('<br/><br/>');
        userMsgBox.val('');
        // XXX: data param not working
        $.get('/~misha/megahal/talk.php?q='+encodeURIComponent(clientMsg), {}, function(res) {
            var resObj = $.parseJSON(res);
            chatbox.append('HAL: '+resObj.reply+'<br/><br/>');
        });
    });
</script>
</body>
</html>
