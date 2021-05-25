echo "Small"
echo "time(us);resMem(kb);shared(kb);priv(kb)" > results/small.csv
for i in 1 2 3 4 5 6
do
 echo "run $i"
 ./fsNotifierDaemon 2>> results/small.csv
done

sed -i 's/small/intermediate/g' fsNotifier.config
echo "Intermediate"
echo "time(us);resMem(kb);shared(kb);priv(kb)" > results/intermediate.csv
for i in 1 2 3 4 5 6
do
 echo "run $i"
 ./fsNotifierDaemon 2>> results/intermediate.csv
done

sed -i 's/intermediate/big/g' fsNotifier.config
echo "Big"
echo "time(us);resMem(kb);shared(kb);priv(kb)" > results/big.csv
for i in 1 2 3 4 5 6
do
 echo "run $i"
 ./fsNotifierDaemon 2>> results/big.csv
done

sed -i 's/big/small/g' fsNotifier.config
