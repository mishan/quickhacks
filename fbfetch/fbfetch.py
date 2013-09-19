# fbfetch.py -- Fetch posts from a Facebook page and convert to RSS format
# Copyright (C) 2013 Misha Nasledov <misha@nasledov.com>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
import re
import time
import mechanize
import urllib
import sys
from bs4 import BeautifulSoup

def scrapePosts(br, postLinks):
    posts = []
    for postLink in postLinks:
        post = scrapeTimelinePost(br, postLink)
        posts.append(post)
        time.sleep(3)
    return posts

def scrapeTimelinePost(br, postURL):
    ret = {}

    print >> sys.stderr, "Scraping "+postURL

    br.open(postURL)
    resp = br.response()

    htmlBody = resp.read()
    soup = BeautifulSoup(htmlBody)

    postTextSpan = soup.find_all('span', class_='userContent')
    if len(postTextSpan) > 0:
        postText = postTextSpan[0].get_text()
        ret['text'] = postText

    linkBlock = soup.find_all('a', class_='pam shareText')
    if len(linkBlock) > 0:
        ## Find the link URL in the onmouseover block
        linkURL = re.search('(http(s?).*)"', linkBlock[0]['onmouseover']).group(1)
        ## Remove backslashes
        linkURL = re.sub(r'\\', '', linkURL)
        ret['link_url'] = linkURL

    postImage = soup.find_all('img', class_='shareMediaPhoto img')
    if len(postImage) > 0:
        ## The image is ref'd in the CSS style
        postImageStyle = postImage[0]['style']
        ## Use regex to find the image and remove URL encoding
        postImageURL = urllib.unquote(re.search(r'(url|src)=(http(s?)[^&]+)(&|\))', postImageStyle).group(2))
        ret['image'] = postImageURL

    postDateBlock = soup.find_all('abbr')[0]
    ## The date has a formatted string and a UNIX timestamp
    postUTime = postDateBlock['data-utime']
    postDate = postDateBlock['title']

    ret['date_fmt'] = postDate
    ret['date_utime'] = postUTime

    return ret

def getPostXML(post):
    itemXML = "<item>\n"
    itemXML += "  <pubDate>"+post['date_fmt']+"</pubDate>\n"
    itemXML += "  <title>"+post['date_fmt']+"</title>\n";
    contentXML = "  <content:encoded>"
    if post.has_key('text'):
        contentXML += post['text']
    if post.has_key('image'):
        contentXML += "<br/>Image: <img src='"+post['image']+"'/>";
    if post.has_key('link_url'):
        contentXML += "<br/>Link: <a href='"+post['link_url']+"'>Link</a>";
    contentXML = contentXML.replace('\n', '');
    contentXML += "</content:encoded>\n";
    itemXML += contentXML + "</item>\n";
    return itemXML.encode("utf-8")


## Main code:
if len(sys.argv) < 2:
    print "Usage: "+sys.argv[0]+" [url file]"
    exit(1)

br = mechanize.Browser()
br.set_handle_robots(False)

postURLs = [];
with open(sys.argv[1], 'r') as f:
    for line in f:
        postURLs.append(line.strip());

posts = scrapePosts(br, postURLs)

for post in posts:
    print getPostXML(post)

exit(0)
