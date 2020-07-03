/*!
 * \name        Tle5012b_reg.h - Library for Arduino for the TLE5012B angle sensor.
 * \author      Infineon Technologies AG (Dr.Olaf Filies)
 * \copyright   2019 Infineon Technologies AG
 * \version     2.0.1
 * \brief       GMR-based angle sensor for angular position sensing in automotive applications
 *              This library include the register read and bit separation function.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "Tle5012b_reg.h"

Tle5012b::Error_t Tle5012b_reg::readSensorType()
{
	Error_t status;
	uint16_t rawData[3] = {0};

	triggerUpdate();

	status = readMoreRegisters(0xE, sensorRegister.registers , UPD_low, SAFE_high);

	status = readMoreRegisters(REG_TCO_Y + 0x3, rawData , UPD_low, SAFE_high);
	sensorRegister.registers[15] = rawData[0];
	sensorRegister.registers[16] = rawData[1];
	sensorRegister.registers[17] = rawData[2];

	status = readMoreRegisters(REG_D_MAG + 0x2, rawData , UPD_low, SAFE_high);
	sensorRegister.registers[18] = rawData[0];
	sensorRegister.registers[19] = rawData[1];

	status = readTempIIFCnt(rawData[0]);
	//TODO: this assignment??
	//sensorRegister.registers[20] = rawData;

	status = readTempT25(rawData[0]);
	//TODO: this assigment??
	//sensorRegister.registers[21] = rawData;

	sensorRegister.stat.reg   = sensorRegister.registers[0];
	sensorRegister.acstat.reg = sensorRegister.registers[1];
	sensorRegister.aval.reg   = sensorRegister.registers[2];
	sensorRegister.aspd.reg   = sensorRegister.registers[3];
	sensorRegister.arev.reg   = sensorRegister.registers[4];
	sensorRegister.fsync.reg  = sensorRegister.registers[5];
	sensorRegister.mod1.reg   = sensorRegister.registers[6];
	sensorRegister.sil.reg    = sensorRegister.registers[7];
	sensorRegister.mod2.reg   = sensorRegister.registers[8];
	sensorRegister.mod3.reg   = sensorRegister.registers[9];
	sensorRegister.offx.reg   = sensorRegister.registers[10];
	sensorRegister.offy.reg   = sensorRegister.registers[11];
	sensorRegister.synch.reg  = sensorRegister.registers[12];
	sensorRegister.ifab.reg   = sensorRegister.registers[13];
	sensorRegister.mod4.reg   = sensorRegister.registers[14];
	sensorRegister.tcoy.reg   = sensorRegister.registers[15];
	sensorRegister.adc.ADCX   = sensorRegister.registers[16];
	sensorRegister.adc.ADCY   = sensorRegister.registers[17];
	sensorRegister.dmag.reg   = sensorRegister.registers[18];
	sensorRegister.traw.reg   = sensorRegister.registers[19];
	sensorRegister.iifcnt.reg = sensorRegister.registers[20];
	sensorRegister.t250.reg   = sensorRegister.registers[21];

	return (status);
}

Tle5012b::Error_t Tle5012b_reg::identifyInterfaceType()
{
	Error_t status = readSensorType();
	if (status != NO_ERROR) {
		return (status);
	}

	sensorRegister.stat.fetch_SNR(sensorRegister.stat.reg);

	sensorRegister.mod1.fetch_IIFMOD(sensorRegister.mod1.reg);
	sensorRegister.mod1.fetch_DSPUHOLD(sensorRegister.mod1.reg);
	sensorRegister.mod1.fetch_CLKSEL(sensorRegister.mod1.reg);
	sensorRegister.mod1.fetch_FIRMD(sensorRegister.mod1.reg);

	sensorRegister.mod2.fetch_AUTOCAL(sensorRegister.mod2.reg);
	sensorRegister.mod2.fetch_PREDICT(sensorRegister.mod2.reg);
	sensorRegister.mod2.fetch_ANGDIR(sensorRegister.mod2.reg);
	sensorRegister.mod2.fetch_ANGRANGE(sensorRegister.mod2.reg);

	sensorRegister.mod3.fetch_PADDRV(sensorRegister.mod3.reg);
	sensorRegister.mod3.fetch_SSCOD(sensorRegister.mod3.reg);
	sensorRegister.mod3.fetch_SPIKEF(sensorRegister.mod3.reg);

	sensorRegister.ifab.fetch_IFADHYST(sensorRegister.ifab.reg);
	sensorRegister.ifab.fetch_IFABOD(sensorRegister.ifab.reg);
	sensorRegister.ifab.fetch_FIRUDR(sensorRegister.ifab.reg);

	sensorRegister.tcoy.fetch_SBIST(sensorRegister.tcoy.reg);

	sensorRegister.mod4.fetch_IFMD(sensorRegister.mod4.reg);
	sensorRegister.mod4.fetch_IFABRES(sensorRegister.mod4.reg);
	sensorRegister.mod4.fetch_HSMPLP(sensorRegister.mod4.reg);

	identify();

	return (status);
}

void Tle5012b_reg::identify(){

	switch (sensorRegister.mod4.IFMD)
	{
		case IIF:
			sensorRegister.interface = IIF;
			sensorRegister.interfaceName = "IIF";
			sensorRegister.sensorBoard = TLE5012B_E1000;
			sensorRegister.sensorName = "TLE5012B_E1000";
			break;
		case PWM:
			sensorRegister.interface = PWM;
			sensorRegister.interfaceName = "PWM";
			if (sensorRegister.ifab.FIRUDR == 0x0 && sensorRegister.ifab.IFABOD == 0x0 )
			{
				sensorRegister.sensorBoard = TLE5012B_E5000;
				sensorRegister.sensorName = "TLE5012B_E5000";
			}else{
				sensorRegister.sensorBoard = TLE5012B_E5020;
				sensorRegister.sensorName = "TLE5012B_E5020";
			}
			break;
		case HSM:
			sensorRegister.interfaceName = "HSM";
			sensorRegister.interface = HSM;
			sensorRegister.sensorBoard = TLE5012B_E3005;
			sensorRegister.sensorName = "TLE5012B_E3005";
			break;
		case SPC:
			sensorRegister.interfaceName = "SPC";
			sensorRegister.interface = SPC;
			sensorRegister.sensorBoard = TLE5012B_E9000;
			sensorRegister.sensorName = "TLE5012B_E9000";
			break;
	}
}

Tle5012b::Error_t Tle5012b_reg::writeInterfaceType(interfaceType iface)
{
	uint16_t rawData = 0;

	Error_t status = readIntMode4(rawData);
	if (status != NO_ERROR) {
		return (status);
	}

	rawData &= ~(1UL << 0);
	rawData &= ~(1UL << 1);
	rawData = rawData | iface;
	status = writeIntMode4(rawData);

	return (status);
}

Tle5012b::Error_t Tle5012b_reg::resetFirmware()
{
	uint16_t rawData = 0x401;
	Error_t status = writeActivationStatus(rawData);
	return (status);
}

Tle5012b::Error_t Tle5012b_reg::setCalibration(calibrationMode calMode)
{
	uint16_t rawData = 0;

	Error_t status = readIntMode2(rawData);
	if (status != NO_ERROR) {
		return (status);
	}

	rawData &= ~(1UL << 0);
	rawData &= ~(1UL << 1);
	rawData = rawData | calMode;
	status = writeIntMode2(rawData);

	return (status);
}

