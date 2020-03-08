import sys
import os
import time

listKeywords=[]
listKeywordsEsc=[]

def DelayedExit():
    time.sleep(2)
    sys.exit()

def StripRemark(s):
  rm = s.find('//')
  if rm >= 0:
    s= s[:rm]
  return s.strip()

def GetLabel(s):
  if '[]' in s:
    return s[s[:s.find('[]')].rfind(' ')+1:s.find('[]')]
  else:
    return ''

def GetMessageText(s):
  b = s.find('"')
  e = s.rfind('"')

  text = s[b + 1:e].strip().replace('\\n', ' ')
  for ch in ['\\','`','*','_','{','}','[',']','(',')','>','#','+','-','.','$','\'',':',',','"','?']:
    if ch in text:
        text = text.replace(ch,' ')

  return text

def GetMessageTextEsc(keyword):
  keywordEsc = ''
  for i in range(0, len(keyword)): 
    keywordEsc = keywordEsc + "'{}',".format(keyword[i])
  return keywordEsc[0:-1]

def GetMessage(s, listKeywordsEsc):
  msg = ''
  esc = False
  b = s.find('"')
  e = s.rfind('"')
  if e-b > 0:
    for i in range(b+1,e):
      if esc:
        if s[i] == 'n':
          msg = msg + "'\\n',"
        else:  
          msg = msg + "'{}',".format(s[i])
        esc = False
      elif s[i] == '\\':
        esc = True
      else:
        if s[i] == "'":
          msg = msg + "'\\'',"
        else:
          msg = msg + "'{}',".format(s[i])
    if msg != '':
      #msg+='0x00,'
      msg = msg[0:-1] + ', 0x80,'

      # check for keywords. 
      for x in range(0, len(listKeywordsEsc)):
        keyword = listKeywordsEsc[x]
        msg = msg.replace(keyword, str(x + 129))


  return msg
    
################################################################################

if len(sys.argv) != 2 :
    print("\nUsage: {} filename\n".format(os.path.basename(sys.argv[0])))
    DelayedExit()
    
filename = os.path.abspath(sys.argv[1])    
with open(filename,"r") as file:
  lines = file.readlines()
  file.close()


# work out the keywords.
keywords = dict()
keywords['elmet'] = 2
keywords['nventory'] = 2
keywords[' of a Kind!'] = 2
keywords['\"#% Yes'] = 2
keywords['\"$% No'] = 2
keywords['188'] = 2
keywords['184'] = 2

for line in lines:
  line = StripRemark(line)
  label = GetLabel(line)
  msgText = GetMessageText(line)

  if (label != '') and (msgText != ''):
    words = msgText.split() #split string into a list

    for word in words:
      if word in keywords:
        keywords[word] += 1
      else:
        keywords[word] = 1



del keywords['Helmet']

# remove singles and short words ..

newKeywords = { k:v for k,v in keywords.items() if v>1 and len(k) > 2 }


newKeywords['kill'] = 2
newKeywords['ress'] = 2
newKeywords['     '] = 2
newKeywords['   '] = 2
newKeywords['  '] = 2
newKeywords['ed '] = 2
newKeywords['ing'] = 2
newKeywords['to '] = 2
newKeywords[' to'] = 2
newKeywords[' of '] = 2
newKeywords['ist'] = 2
newKeywords['ll'] = 2
newKeywords['ive'] = 2
newKeywords['ite'] = 2
newKeywords['est'] = 2
newKeywords['ore'] = 2
newKeywords['ion'] = 2
newKeywords['ave'] = 2
newKeywords['ies'] = 2
newKeywords['ics'] = 2
newKeywords['ump'] = 2
# newKeywords['es'] = 2
newKeywords['ea'] = 2
# newKeywords['at'] = 2
newKeywords['ou'] = 2
# newKeywords['an'] = 2
newKeywords['in'] = 2
# newKeywords['on'] = 2
# newKeywords['or'] = 2
newKeywords['us'] = 2
# newKeywords['as'] = 2
newKeywords['re'] = 2
# newKeywords['ed'] = 2
newKeywords['se'] = 2
newKeywords['er'] = 2
newKeywords['ar'] = 2
# newKeywords['ur'] = 2
# newKeywords['st'] = 2
# newKeywords['op'] = 2
# newKeywords['am'] = 2
newKeywords[' do '] = 2
newKeywords['Th'] = 2
newKeywords['a '] = 2

listKeywords = [k for k,v in newKeywords.items()]
for keyword in listKeywords:
  listKeywordsEsc.append(GetMessageTextEsc(keyword))

print(listKeywordsEsc)


labels = []
messages = []
for line in lines:
  line = StripRemark(line)
  label = GetLabel(line)
  msg = GetMessage(line, listKeywordsEsc)
  msgText = GetMessageText(line)
  if (label != '') and (msg != ''):
    labels.append(label)
    messages.append(msg)
if len(labels) < 255:
  type ="constexpr uint8_t"
else:
  type ="constexpr uint16_t"  
with open(os.path.dirname(filename)+os.sep+'messages-constants.h','w') as file:
  file.write('#pragma once\n\n// Imported {} messages from "{}"\n\n'.format(len(messages),os.path.basename(sys.argv[1])))
  for i in range (len(labels)):
    file.write('{} {}_Idx = {};\n'.format(type,labels[i],i))

  file.write('\nconst uint8_t keywords[] PROGMEM = {\n')
  for i in range (len(listKeywordsEsc)):
    file.write('{} + 0x80, // {}\n'.format(listKeywordsEsc[i], str(i + 129)))
  file.write('};\n\n')

  file.write('\nconst uint8_t messages[] PROGMEM = {\n')
  for i in range (len(messages)):
    file.write('{}\n'.format(messages[i]))
  file.write('};\n')
  file.close()