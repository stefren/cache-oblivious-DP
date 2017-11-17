import random

characters = ["A", "B", "C", "D", "E"]
length = 100
f = open('gen_input', 'w')
f.write(str(length)+"\n")
f.write(str(length)+"\n")

for i in range(2):
	for i in range(length):
		random_index = random.randint(0, len(characters)-1)
		f.write(characters[random_index])
	f.write("\n")
	