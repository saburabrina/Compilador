cont=1
for i in teste/*.in
do
	./compilador.exe < $i > teste/$cont
	let cont=cont+1
done