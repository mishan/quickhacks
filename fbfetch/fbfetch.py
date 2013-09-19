# Copyright (C) 2013 Misha Nasledov <misha@nasledov.com>
# All Rights Reserved.
import re
import mechanize
import urllib
from bs4 import BeautifulSoup

def scrapePosts(br, postLinks):
    posts = []
    for postLink in postLinks:
        posts.append(scrapeTimelinePost(br, postLink))
    return posts

def scrapeTimelinePost(br, postURL):
    ret = {}

    br.open(postURL)
    resp = br.response()

    htmlBody = resp.read()
    soup = BeautifulSoup(htmlBody)

    postText = soup.find_all('span', class_='userContent')[0].get_text()

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
    ret['text'] = postText

    return ret

## Main code:
br = mechanize.Browser()
br.set_handle_robots(False)

postURLs = ['https://www.facebook.com/ThinkBeforeYouBounce/posts/220284061385346',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/219966181417425',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/203682949720845',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/198559480231304',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/148767168559788',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/196011253816071',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/143305695773353',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/223589964377073',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/315851401763742',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/308290615866908',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/307026192660017',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/304639572898679',
         'https://www.facebook.com/ThinkBeforeYouBounce/posts/297665740245428']

posts = scrapePosts(br, postURLs)

for post in posts:
    print post['date_fmt']
    print post['text']
    if post.has_key('image'):
        print post['image']
    if post.has_key('link_url'):
        print post['link_url']
    print
