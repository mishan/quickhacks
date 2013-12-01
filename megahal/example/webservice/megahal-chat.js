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
            $('.megahal-chatbox').html(getHALText(resObj.reply));
        }
    });

    $('.input-form').submit(function(e) {
        var userMsgBox = $('.user-input');
        var clientMsg = userMsgBox.val();
        var chatbox = $('.megahal-chatbox');

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
