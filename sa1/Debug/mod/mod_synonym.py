from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.common.exceptions import NoSuchElementException  
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
import random
import os

driver = webdriver.Chrome("C:/chromedriver.exe")

CONST_LINK        = "https://www.thesaurus.com/"
CONST_INPUT_CLASS = "wVSRkuvMyimSq0t3IhbM"
CONST_SEARCH_TERM = "angry"
CONST_SYN_CLASS   = "css-1kg1yv8 eh475bn0"

def main():
    try:
        driver.get(CONST_LINK)

        inputField_P   = driver.find_element(By.CLASS_NAME, CONST_INPUT_CLASS)
        inputField_Obj = inputField_P.find_element(By.TAG_NAME, "input")

        inputField_Obj.send_keys(CONST_SEARCH_TERM)
        inputField_Obj.submit()
        
        synList = driver.find_elements(By.TAG_NAME, "li")
        synTextList = []

        # get text from synList
        for x in range(0, len(synList)):
            try:
                text = synList[x].text
                synTextList.append(text)
            except:
                continue

        if(len(synTextList) > 0):
            rndIndex = random.randrange(75, len(synTextList))
            print(synTextList[rndIndex])

    except Exception as e:
        print(str(e))

if __name__ == "__main__":
    main()