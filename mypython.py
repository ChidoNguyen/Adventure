#Chido Nguyen 931-50-6965 ; Exploring Python Program ; Python is new to me this whole program was done using google / learnxiny / and python manual look ups
import random
def main():
	for i in range(3):
		A='chido'
		B=str(i)
		C=A+B
		lc_str=""
		f= open(C,"w")
		for i in range(10):
			random_letter=chr(random.randrange(97,123))
			lc_str+=random_letter
		print(lc_str)
		f.write("%s\n" % lc_str)
		f.close()

	n1 = random.randrange(1,43)
	n2 = random.randrange(1,43)
	print("%i" % n1)
	print("%i" % n2)
	n3 = n1 * n2
	print("%i" % n3)
if __name__ == '__main__':
	main()
