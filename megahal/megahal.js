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

var ffi = require('ffi');

var libmegahal = ffi.Library('./libmegahal', {
    // These must be called before megahal_initialize()
    'megahal_setnobanner':      [ 'void',   [ ] ],
    'megahal_setnoprompt':      [ 'void',   [ ] ],
    'megahal_setnowrap':        [ 'void',   [ ] ],
    // params: filename
    'megahal_seterrorfile':     [ 'void',   [ 'string' ] ],
    // params: filename
    'megahal_setstatusfile':    [ 'void',   [ 'string' ] ],
    // params: directory
    'megahal_setdirectory':     [ 'void',   [ 'string' ] ],

    // This must be called before using megahal.
    'megahal_initialize':       [ 'void',   [ ] ],

    // Returns an initial greeting from the bot
    'megahal_initial_greeting': [ 'string', [ ] ],

    // params: input, log (whether to write to megahal.txt)
    // return: reply
    'megahal_do_reply':         [ 'string', ['string', 'int'] ],
    // params: input, log (whether to write to megahal.txt)
    'megahal_learn_no_reply':   [ 'void',   ['string', 'int'] ],

    // This must called when done to ensure everything is saved.
    'megahal_cleanup':          [ 'void',   [ ] ],
});

function MegaHAL(optParams) {
    this.log = 0;
    if (optParams) {
        // TODO: other params?
        if (optParams.directory !== undefined) {
            libmegahal.megahal_setdirectory(optParams.directory);
        }
        if (optParams.writeLog !== undefined) {
            this.log = optParams.writeLog;
        }
    }
}

MegaHAL.prototype.setErrorFile = function(filename) {
    libmegahal.megahal_seterrorfile(filename);
}

MegaHAL.prototype.setStatusFile = function(filename) {
    libmegahal.megahal_setstatusfile(filename);
}

MegaHAL.prototype.setDirectory = function(dir) {
    libmegahal.megahal_setdirectory(dir);
}

MegaHAL.prototype.init = function() {
    libmegahal.megahal_setnobanner();
    libmegahal.megahal_setnoprompt();
    libmegahal.megahal_setnowrap();
    libmegahal.megahal_initialize();
}

MegaHAL.prototype.greet = function() {
    return libmegahal.megahal_initial_greeting();
}

MegaHAL.prototype.learn = function(input) {
    libmegahal.megahal_learn_no_reply(input, this.log);
}

MegaHAL.prototype.reply = function(input) {
    return libmegahal.megahal_do_reply(input, this.log);
}

MegaHAL.prototype.save = function() {
    libmegahal.megahal_cleanup();
}

module.exports.MegaHAL = MegaHAL;
