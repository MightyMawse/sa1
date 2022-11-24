
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.common.exceptions import NoSuchElementException  
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
import os

CONST_LINK_CLASS = 'yuRUbf'
CONST_SRCH_NAME = 'q'
CONST_DRIVER_PATH = 'F:/chromedriver.exe'
CONST_WRITE_FILE = "F:/scrape.txt"
CONST_KEYWORD_FILE = "F:/keyword.txt"

invalidTerms = { '', 'www', '.com', '.gov'}

driver = webdriver.Chrome(CONST_DRIVER_PATH)

savedPageIndex = 0
searchTerm = None

def main():
    driver.get('http://www.google.com')
    searchTerm = ReadKeyWord()
    try: 
        driver.implicitly_wait(10) # wait for page to load

        searchBar = driver.find_element(By.NAME, CONST_SRCH_NAME)
        searchBar.send_keys(searchTerm)
        searchBar.submit() # search

        pageList = driver.find_elements(By.CLASS_NAME, CONST_LINK_CLASS)
        linkList_href = []

        # compile list of all hrefs
        for n in range(0, len(pageList)):
            href = pageList[n].find_element(By.TAG_NAME, "a")
            linkList_href.append(href.get_attribute("href"))

        file = open(CONST_WRITE_FILE, 'w').close()

        for x in range(0, len(linkList_href)): # goto each site
            driver.get(linkList_href[x])
            driver.implicitly_wait(20)

            # Get text
            bodyText1 = driver.find_elements(By.TAG_NAME, "p")
            #bodyText2 = driver.find_elements(By.TAG_NAME, "textarea")
            #bodyText3 = driver.find_elements(By.TAG_NAME, "a")

            #ls = { bodyText1, bodyText2, bodyText3 }

            compiled = ""
            for i in range(0, len(bodyText1)): # Add all text to compiled text var
                #for c in range(0, len(ls[i])):
                #if(CheckValid(bodyText1[i].text)): # Check if term is illegal
                compiled += bodyText1[i].text

            WriteToFile(compiled, (True if x == 0 else False)) # write text to file

    except Exception as e:
        print(str(e))
        
def ReadKeyWord():
    try:
        file = open(CONST_KEYWORD_FILE, 'r')
        content = file.read()
        return content
    except Exception as e:
        print(str(e))

def WriteToFile(contents, clear):
    try:
        file = open(CONST_WRITE_FILE, ("w" if clear else "a"))
        file.write(contents)
        file.close()
    except Exception as e:
        print(str(e))

def CheckValid(term): # Check if term should be excluded
    for n in range(0, len(invalidTerms)):
        if invalidTerms[n] in term:
            return False
    return True

if __name__ == "__main__":
    main()