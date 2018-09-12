age = input("How old are you?: ")
if age > 60:
    print "If you're", age, "years old then why the **** are you clicking my python script?"
if age < 60 and age > 18:
    print "Good for you..."
if age < 18:
    print "You're", age, "shouldn't you be doing homework or something?"
import os
os.system("pause")
