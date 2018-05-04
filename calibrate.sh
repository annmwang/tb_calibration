tdo_file=/Users/annwang/atlas/testbeam/CALIBRATIONS/PT100_TAC60_TDO.root
pdo_file=/Users/annwang/atlas/testbeam/CALIBRATIONS/PT100_TAC60_PDO.root
xadc_file=/Users/annwang/atlas/testbeam/CALIBRATIONS/xADC.root

#make
echo "calibrating TDO"
#./Fit_TDO ${tdo_file} -o ${tdo_file/\.root/}_fit.root
#./Calibrate_TDO ${tdo_file/\.root/}_fit.root -o ${tdo_file/\.root/}_calib.root

echo "calibrating xADC"
#./Fit_xADC ${xadc_file} -o ${xadc_file/\.root/}_fit.root
#./Calibrate_xADC ${xadc_file/\.root/}_fit.root -o ${xadc_file/\.root/}_calib.root

echo "calibrating PDO"
./Fit_PDO ${pdo_file} -o ${pdo_file/\.root/}_fit.root
./Calibrate_PDO ${pdo_file/\.root/}_fit.root -o ${pdo_file/\.root/}_calib.root -x ${xadc_file/\.root/}_calib.root
