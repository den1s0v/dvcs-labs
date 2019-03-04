import argparse
import datetime
import os
import re
import shutil
from urllib.request import urlopen
import urllib.parse

from bs4 import BeautifulSoup

# limit for names (with ext.) of files to be saved
MAX_FILENAME_LENGTH = 60


def terminate_with_error(err_str):
    print('Error:')
    print('\t', err_str)
    exit()


def show_warning(warn_str):
    print('Warning:')
    print('\t', warn_str)


def write_url_error_to_log(log_fd, url):
    msg = "[%s] Can`t get:  %s\n" % (
        datetime.datetime.now().replace(microsecond=0).isoformat(sep=' '),
        url,
    )
    log_fd.write(msg)


def parse_cmdline():
    """Parses command line arguments and returns an object with fields (of type string):
        url         (for -u option)
        destination (for -d option)
        log_file    (for -l option)
    
    """
    parser = argparse.ArgumentParser(
        description='Download all graphic files (.png, .jpeg, .jpg) from a webpage.'
    )
    parser.add_argument("--url", "-u", 
                        help="webpage source URL", 
                        required=True)
    parser.add_argument("--destination", "-d", 
                        default='./images', 
                        help="destination directory where to save images (default: ./images)", 
                        required=False)
    parser.add_argument("--log-file", "-l", 
                        default='./downloader-errors.log', 
                        help="path to file used for logging errors (default: ./downloader-errors.log)", required=False)
    args = parser.parse_args()

    if args.url and args.destination and args.log_file:
        # normalize paths
        args.destination = os.path.normpath(args.destination)
        args.log_file = os.path.normpath(args.log_file)
        # report parameters
        print('Webpage Images Downloader is started with parameters:')
        print('Using URL:\t',args.url)
        print('Dir to write:\t',args.destination)
        print('Log file:\t',args.log_file)
        print()
    
    return args


def prepare_dir(dir):
    """Checks the path and creates a directory if nesessary.
    If path does exist and is not a directory,
    or specified location is not writable, an exception is trown.
    
    """
    if not os.path.exists(dir):
        # create a directory tree
        try:
            os.makedirs(dir, exist_ok=True)
        except Exception as e:
            terminate_with_error(str(e) + '\n\tOccured while preparing destination dir.')
        print('destination dir created.')
    else:
        # check write permission
        if not os.access(dir, os.F_OK | os.W_OK):
            terminate_with_error("directory is not accessible or writable: "+dir)


def fetch_url(url, terminate_on_error=False, quiet=False):
    """Gets bytes via given URL `url` and reports errors if any.
    If `terminate_on_error` is True, program will be interrupted with an error.
    If `quiet` is True, nothing will be reported.
    
    """
    try:
        with urlopen(url) as f:
            web_doc = f.read()
    except Exception as e:
        web_doc = None
        if terminate_on_error:
            terminate_with_error(str(e))
        elif not quiet:
            show_warning(str(e))
    
    return web_doc


def url2path(url, keep_subpath=False):
    """Converts `url` into relative path. """
    path = urllib.parse.urlsplit(url).path
    if not keep_subpath:
        # split  path into 2 parts: dir and filename, take filename only
        path = os.path.split(path)[1]
        
    return path


def fit_filename(path):
    """Makes path valid: removes unsuported chars and renames if such file exists. """
    dir, filename = os.path.split(path)
    name, ext = os.path.splitext(filename)

    # remove extra chars
    name = re.sub(r'(?:%\d+)|[^a-z\s\d.,!@#$%\(\)=+_-]+', r'', name, flags=re.IGNORECASE).strip()
    name = re.sub(r'\s+',r'_', name)
    filename = name + ext
        
    # shrink filename if too long
    if(len(filename) > MAX_FILENAME_LENGTH):
        name = name[0:MAX_FILENAME_LENGTH-len(ext)]
        
    path = os.path.join(dir, name + ext)
        
    if not os.path.exists(path):
        return path
        
    # rename if exists
    root, ext = os.path.splitext(path)
    count = 1
    while True:  # `do..while` emulation
        path = root + ' ('+str(count)+')' + ext
        if not os.path.exists(path):
            break   
        count += 1

    return path


def get_save_path(dir, url):
    filepath = os.path.join(dir, url2path(url))
    return fit_filename(filepath)


def download_image(img_url, save_path, log_fd=None, webpage_url=None):
    """Gets file data via given URL `img_url` 
    and saves it to specified `save_path`.
    If file descriptor `log_fd` opened for writing is specified,
    then requesting errors will be logged there.
    If `img_url` occures to be relative URL, 
    an absolute URL `webpage_url` can be used 
    to attempt repair the `img_url`, i.e. make it absolute.
    
    """
    # print(ascii(save_path))
    print('collecting ', os.path.split(save_path)[1],'...', end='\t')
    
    file_data = fetch_url(img_url, quiet=bool(webpage_url))
    
    if not file_data and webpage_url:
        # try to repair URL (expand possibly relative URL)
        img_url = urllib.parse.urljoin(webpage_url, img_url)
        # fetch again
        file_data = fetch_url(img_url)
    
    if file_data:
        with open(save_path,'wb') as imgfile:
            imgfile.write(file_data)
        print(str(round(len(file_data)/1024, 2)), 'KB', 'OK')
    elif log_fd:
        write_url_error_to_log(log_fd, img_url)


def download_images(url, save_dir, log_fd):
    """Gets an HTML of given URL `url` and parses it to
    extract all picture URLs from <img src='...'> tags.
    Then tries to download all found .png, .jpg, .jpeg files
    and saves them to specified `save_dir`.
    If file descriptor `log_fd` opened for writing is specified,
    then requesting errors will be logged there.
    
    The function is main function of this module.
    
    """
    print('Requesting webpage ...')
    html_doc = fetch_url(url, terminate_on_error=False)
    if not html_doc:
        if log_fd:
            write_url_error_to_log(log_fd, url)
        terminate_with_error('Cannot access the webpage, please check internet connection and URL.')
   
    print('Extracting links ...')
    soup = BeautifulSoup(html_doc, features="html.parser")
    del html_doc

    img_urls = [img.get('src') for img in soup.find_all('img')]
    del soup
    
    # filter URLs by file extension
    allowed_extensions = ['png','jpeg','jpg',]
    ext_tester = re.compile(
        r'(?:' + '|'.join([r'\.'+ext for ext in allowed_extensions]) + ')$',
        re.IGNORECASE
    )
    img_urls = [url for url in img_urls  if ext_tester.search(url)]
    print(len(img_urls), 'image URLs found')

    # remove URL duplicates and sort
    img_urls = list(set(img_urls))
    img_urls.sort()
    print(len(img_urls), 'unique URLs')

    print('Downloading images ...')
    for img_url in img_urls:
        save_path = get_save_path(save_dir, img_url)
        download_image(img_url, save_path, log_fd, webpage_url=url)



if __name__ == '__main__':
            
    args = parse_cmdline()
    prepare_dir(args.destination)
    
    with open(args.log_file, 'a', errors='replace') as log_fd:
        download_images(args.url, args.destination, log_fd)
    
    print('Done!')


""" Test URLs
https://www.crummy.com/software/BeautifulSoup/bs4/doc/
http://python-3.ru
https://en.wikipedia.org/wiki/Incandescent_light_bulb
https://cameralabs.org/4294-samye-krasivye-gory-so-vsego-mira-117-fotografij
"""
