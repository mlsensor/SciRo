
sfreq=2; % downsampling frequency.
lag=100;% cut off the beginning part.
siz=length(outp)/1-lag-100; % size of posp (position matrix)..
rx=[1:6]; % which RX pairs of the LCR to use.


% Downsampling the actual frequency by sfreq.
% initial frequency: 20 (from sampling_LCR_multi_triggered.m) divided by 2
% (sfreq) = 10 Hz.
inpf=inp(1:sfreq:end);
outpf=outp(1:sfreq:end,:);
pospf=posp(:,1:sfreq:end,:); 
siz=siz/sfreq;
offset=0;

% combining pressure values (inpf) and sensor values (outpf).
x=[inpf(3+lag+offset:siz+2+offset,:),outpf(3+lag+offset:siz+2+offset,rx)]';
%x=[outpf(3+lag+offset:siz+2+offset,rx)]';
%x=[inpf(3+lag+offset:siz+2+offset,2)]';




% subtracting second marker from the first in order to determine the
% relative positioning. Replace with load cell values for force prediction
t=(squeeze(pospf(:,3+lag:siz+2,2))-squeeze(pospf(:,3+lag:siz+2,1)));
%t=[(squeeze(pospf(:,3+lag:siz+2,2))-squeeze(pospf(:,3+lag:siz+2,1))) ;outpf(3+lag:siz+2,7)' ];
%t=[inpf(2+lag:siz+1,1)]';

%% Build training and testing sets.
% Number of inputs/outputs.
inputSize = size(x,1);
numResponses = size(t,1);

% Where to split the data between testing and training..

divi=floor(0.8*length(x));
numTimeStepsTrain=divi;

%numTimeStepsTrain = floor(0.9*numel(data));
% x=normalize(x,2);
% t=normalize(t,2);

xm=mean(x');
xs=std(x');
tm=mean(t');
ts=std(t');

% Normalizing by mean and stdev for both inputs/outputs.
for z=1:inputSize
    x(z,:)= x(z,:)-xm(z);
     x(z,:)= x(z,:)/xs(z);
end
for z=1:numResponses
        t(z,:)= t(z,:)-tm(z);
     t(z,:)= t(z,:)/ts(z);
end

% Split the data based on the index.
XTrain = x(:,1:divi);
YTrain = t(:,1:divi);
XTest = x(:,divi+1:end);
YTest = t(:,divi+1:end);


%% Parameters for LSTM.
numHiddenUnits = 50;
% Computational capability/complexity of the network is picked via trial and
% error. As small as possible to prevent overfitting.

layers = [ ...
    sequenceInputLayer(inputSize)
    dropoutLayer(0.1) %dropout should prevent overfitting and make predicitons more robust to noise
    lstmLayer(numHiddenUnits)%,'OutputMode','last'
    fullyConnectedLayer(numResponses)
    regressionLayer];
%bilstmLayer

opts = trainingOptions('adam', ...
    'MaxEpochs',2000, ... % number of training iterations.
    'MiniBatchSize', 512,... %%%512
    'GradientThreshold',1, ...
    'InitialLearnRate',0.005*1, ...
    'LearnRateSchedule','piecewise', ...
    'LearnRateDropPeriod',125*1, ...%%changed
    'LearnRateDropFactor',0.2/1, ...%%
    'Verbose',0, ...
    'Plots','training-progress', 'ExecutionEnvironment', 'gpu');

% Training.
net = trainNetwork(XTrain,YTrain,layers,opts);

% Predict for the whole dataset.
[net,YPred_o]= predictAndUpdateState(net,x);
%[net,YPred] = predictAndUpdateState(net,XTrain(:,end));

% Undo the normalization.
for z=1:numResponses
     t(z,:)= t(z,:)*ts(z);
     YPred_o(z,:)=YPred_o(z,:)*ts(z);
end

% Plotting.
%plot(YPred_o(3,:),'r')
plot(YPred_o(1,:),'r')
hold on
plot(t(1,:),'b')
% figure;
% plot(YPred_o(4,:),'r')
% hold on
% plot(t(4,:),'b')

% Error. 
err=rssq(YPred_o(1:3,:)-t(1:3,:));
test_s=0.8*length(err);
mean(err(test_s:end)) % mean error for only the test set.
std(err(test_s:end))



