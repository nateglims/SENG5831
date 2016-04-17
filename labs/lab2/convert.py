t = list()
pr = list()
pm = list()

with open('slow_rate_raw_data.txt') as f:
    for line in f:
        if line.startswith('T'):
            t.append(line.split()[-1].strip())
        elif line.startswith('Pm'):
            pm.append(line.split()[-1].strip())
        elif line.startswith('Pr'):
            pr.append(line.split()[-1].strip())

with open('slow_rate_data.csv', 'w') as f:
    f.write("T, Pr, Pm\n")

    for (tor, ref, mot) in zip(t, pr, pm):
        f.write("%s, %s, %s \n" % (tor, ref, mot))
