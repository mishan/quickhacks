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

var bot = new megahal.MegaHAL({writeLog: 0});
bot.init();
var stdin = process.openStdin();

stdin.addListener('data', function(d) {
    if (d == "\n") {
        bot.save();
        process.exit(0);
    } else {
        console.log(bot.reply(d.toString()));
    }
});
