import os
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from time import sleep

driver = webdriver.Chrome("F:/chromedriver.exe")
CONST_SITE_NAME = "https://grammica.com/plagiarism-checker"
CONST_SET_NAME = "F:/scrape.txt"
CONST_BTN_CLASS = "btn btn-3d btn-primary"
CONST_FIT_NAME = "F:/fitness.txt"

def main():
    driver.get(CONST_SITE_NAME)
    textArea = driver.find_element(By.TAG_NAME, "textarea")
    textArea.send_keys(GetCSet())

    btn = driver.find_elements(By.TAG_NAME, "button")
    for n in range(0, len(btn)):
        if "Check Plagarism" in btn[n].text:
            btn[n].submit()

    while(True):
        try:
            strPl = driver.find_element(By.ID, "plagiarized-percentage-text")

            if strPl.text != "0%":
                sleep(20)
                SetCSetFitness(strPl.text)
        except:
            continue

def GetCSet():
    try:
        file = open(CONST_SET_NAME, 'r')
        content = file.readlines()
        return content
    except:
        return None

def SetCSetFitness(score):
    file = open(CONST_FIT_NAME, 'a')
    file.write(score)
    file.close()


if __name__ == "__main__":
    main()



