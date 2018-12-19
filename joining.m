% This file is used to merge two sets of
% vectors (inp, outp, posp) between two workspaces (.mat files) for testing
% different combinations of data for training. Load dataset1, press play,
% load dataset2, press play --> and the data will be joined. When you are
% done, manually run the final part.

if exist('a1','var') == 0
    siz=length(outp)/1-100; % cut off the end.
    
    %load('t3_4contcts.mat','inp','outp','posp')
    %outp=outp-outp(1,:);
    % outp=normalize(outp,2);
    
%     outp=outp-mean(outp(10:100,:));
%     outp=outp./range(outp(10:100,:));
    %
    inp=inp(10:siz,1);
    outp=outp(10:siz,:);
    posp=posp(:,10:siz,:);
    
    a1=[inp];
    b1=[outp];
    c1=posp;
else
    
    siz=length(outp)-100; % cut off the end.
    
    %load('t3_4contcts.mat','inp','outp','posp')
    %outp=outp-outp(1,:);
    %  outp=normalize(outp,2);
    %
%     outp=outp-mean(outp(10:100,:));
%     outp=outp./range(outp(10:100,:));
%     
    inp=inp(10:siz,1);
    outp=outp(10:siz,:);
    posp=posp(:,10:siz,:);
    
    a1=[a1; inp];
    b1=[b1; outp];
    c1=cat(2,c1, posp);
end


%% Restore to original file name.
if 0
    inp=a1;
    outp=b1;
    posp=c1;
    
          pl_hold= posp(:,:,2);
        posp(:,:,2)= posp(:,:,1);
       posp(:,:,1)=pl_hold;
end