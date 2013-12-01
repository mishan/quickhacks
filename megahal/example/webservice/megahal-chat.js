function getHALText(text) {
    return '<font color="red">HAL: </font>'+text+'<br/><br/>';
}

function getUserPrefix() {
    return '<font color="blue">User: </font>';
}

$(document).ready(function() {
    // get greeting
    $.get('/~misha/megahal/talk.php', {}, function(res) {
        if (res) {
            var resObj = $.parseJSON(res);
            $('.megahal-chatbox').html(getHALText(resObj.reply));
        }
    });

    // handle user input
    $('.input-form').submit(function(e) {
        var userMsgBox = $('.user-input');
        var clientMsg = userMsgBox.val();
        var chatbox = $('.megahal-chatbox');

        // append user input
        chatbox.append(getUserPrefix(''));
        // escape HTML
        $(document.createTextNode(clientMsg)).appendTo(chatbox);
        chatbox.append('<br/><br/>');
        // reset input
        userMsgBox.val('');
        // query bot
        $.get('/~misha/megahal/talk.php?q='+encodeURIComponent(clientMsg), {}, function(res) {
            if (res) {
                var resObj = $.parseJSON(res);
                chatbox.append(getHALText(resObj.reply));
            }
            // scroll to bottom
            chatbox.animate({
                scrollTop: chatbox.height()
            }, 300);
        });

        // don't do form submit
        return false;
    });
});
