/* Copyright (C) 2013 Misha Nasledov <misha@nasledov.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the license or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the Gnu Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA */
var megahal = require('./megahal.js');
var http = require('http');
var url = require('url');

var bot = new megahal.MegaHAL({writeLog: 0});
bot.init();

http.createServer(function (request, response) {
    if (request.method=='GET') {
        var url_parts = url.parse(request.url,true);
        var reply;
        if (url_parts.query !== undefined && url_parts.query.q !== undefined) {
            reply = bot.reply(url_parts.query.q);
        } else {
            reply = bot.greet();
        }
        response.writeHead(200, {'Content-Type': 'application/json'});
        response.write(JSON.stringify({'reply': reply}));
        response.end();
        bot.save();
    }
}).listen(8787);
