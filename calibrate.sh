#tdo_file=/Users/annwang/atlas/testbeam/CALIBRATIONS/PT100_TAC60_TDO.root
#pdo_file=/Users/annwang/atlas/testbeam/CALIBRATIONS/PT100_TAC60_PDO.root
#xadc_file=/Users/annwang/atlas/bb5/bb5Runs/calibrations_2018_06_14/ser_029/xADC_ser_029_ip05.root
xadc_file=/Users/annwang/atlas/bb5/bb5Runs/calibrations_2018_06_14/xADC_Run114.root
#tdo_file=/Users/annwang/atlas/bb5/bb5Runs/calibrations_2018_06_14/ser_316/TDO_ser_316_ip02.root
#pdo_file=/Users/annwang/atlas/bb5/bb5Runs/calibrations_2018_06_14/ser_029/PDO_ser_029_ip05_bench.root
#pdo_file=/Users/annwang/atlas/bb5/bb5Runs/calibrations_2018_06_14/ser_012/PDO_ser_012_ip04_bench.root
pdo_file=/Users/annwang/atlas/bb5/bb5Runs/calibrations_2018_06_14/PDO_Run114_bench.root
make

echo "calibrating TDO"
#./Fit_TDO ${tdo_file} -o ${tdo_file/\.root/}_fit.root
#./Calibrate_TDO ${tdo_file/\.root/}_fit.root -o ${tdo_file/\.root/}_calib.root

echo "calibrating xADC"
#./Fit_xADC ${xadc_file} -o ${xadc_file/\.root/}_fit.root
#./Calibrate_xADC ${xadc_file/\.root/}_fit.root -o ${xadc_file/\.root/}_calib.root

echo "calibrating PDO"
./Fit_PDO ${pdo_file} -o ${pdo_file/\.root/}_fit.root
echo "finished fitting, now for real calibrating"
./Calibrate_PDO ${pdo_file/\.root/}_fit.root -o ${pdo_file/\.root/}_calib.root -x ${xadc_file/\.root/}_calib.root
