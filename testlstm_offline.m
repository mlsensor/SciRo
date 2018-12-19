 siz=length(outp)/2-100;
 % siz=1000;
 sfreq=1;
% lag=10; % cut off the beginning part.
% %
% % rx=1:2;


%inp(4+lag:siz+3,1),
%x=[inp(3+lag:siz+2,1),outp(1+lag:siz,2),outp(2+lag:siz+1,2)]';%,outp(lag:siz-1,1)
%outp=outp-outp(1,:);

% Downsampling the actual frequency by sfreq.
% initial frequency: 20 (from sampling_LCR_multi_triggered.m) divided by 2
% (sfreq) = 10 Hz.
inpf=inp(1:sfreq:end); 
outpf=outp(1:sfreq:end,:);
pospf=posp(:,1:sfreq:end,:);
siz=siz/sfreq;




%x=[inpf(3+lag:siz+2,1),outpf(3+lag:siz+2,rx),outpf(2+lag:siz+1,rx)]';

%x=[inpf(3+lag:siz+2,1)]';
% combining pressure values (inpf) and sensor values (outpf).
x=[inpf(3+lag:siz+2,1),outpf(3+lag:siz+2,rx)]';


% subtracting second marker from the first in order to determine the
% relative positioning.
%t=squeeze(pospf(1,3+lag:siz+2,2));
%t=rssq(squeeze(pospf(:,3+lag:siz+2,2))-squeeze(pospf(:,3+lag:siz+2,1)));
t=(squeeze(pospf(:,3+lag:siz+2,2))-squeeze(pospf(:,3+lag:siz+2,1)));

%% Build training and testing sets.
% Number of inputs/outputs.
inputSize = size(x,1);
numResponses = size(t,1);

% Where to split the data between testing and training..
%t=t+60*randn(1,length(t));
divi=floor(0.8*length(x));
numTimeStepsTrain=divi;

%numTimeStepsTrain = floor(0.9*numel(data));
% x=normalize(x,2);
% t=normalize(t,2);

% Normalizing by mean and stdev for both inputs/outputs.
for z=1:inputSize
    x(z,:)= x(z,:)-xm(z);
    x(z,:)= x(z,:)/xs(z);
end
for z=1:numResponses
    t(z,:)= t(z,:)-tm(z);
    t(z,:)= t(z,:)/ts(z);
end

% Same as above but without the explicit computation of the mean and stdev:
%x=normalize(x,2);
%t=normalize(t,2);
%x([2:3 6:7],1:end)=0;
%x([2:7],1:end)=0;
%x([4:5],500:end)=x([4:5],500:end)+1;
x([1],1:end)=0;
% Given the input, predict the output using LSTM.
tic
[net,YPred_o ]= predictAndUpdateState(net,x);
toc
%[net,YPred] = predictAndUpdateState(net,XTrain(:,end));

% Undo normalization.
for z=1:numResponses
    
    t(z,:)= t(z,:)*ts(z); % missing the mean?
    YPred_o(z,:)=YPred_o(z,:)*ts(z); % missing the mean?
end

subplot(3,1,1)
plot(YPred_o(3,:))
hold on
plot(t(3,:))

subplot(3,1,2)
plot(YPred_o(2,:))
hold on
plot(t(2,:))

subplot(3,1,3)
plot(YPred_o(3,:))
hold on
plot(t(3,:))


% Error. 
err=rssq(YPred_o-t);
%err(err>50)=0;
mean(err (1:4000))
std(err(1:4000))
%mean(err(round(0.8*length(x)):end))
%std(err(round(0.8*length(x)):end))
M = movmean(err,100);
diffx=diff(t(1,:));
for i=1:length(err)-1
    if diffx(i)>0
        errd(i)=err(i);
    else
        errd(i)=-err(i);
    end
end

figure;
scatter(-t(3,:),-t(2,:),10,(err),'filled')
colormap jet
h = colorbar;
ylabel(h, 'Error[mm]','FontSize',14)
h.FontSize=14;
axis tight
axis equal
axis off
% figure;
% histogram(errd)

% Plotting.
% figure;
% scatter3(t(1,:),t(2,:),t(3,:))
% hold on
% scatter3(YPred_o(1,:),YPred_o(2,:),YPred_o(3,:))