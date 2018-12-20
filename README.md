# Soft robot perception using embedded soft sensors and recurrent neural networks
code and data for the SciRo paper.

The sampling of the sensor values are done using 'sampling_LCR_multi_triggered.m'. The arduino codes for the actuator controller and the multiplexer is given separately. The code is specifically made for data acquisition with the LCR meter. 

'joining.m' is used to stitch multiple samples obtained at different times. 'lstmnew.m' is used for training the kinematics/force estimation network. 'testlstm_LCR_multi_triggered.m' is used for online evaluation of the trained network. 
