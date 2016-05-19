close all
figure()
fileID = fopen('ev6.flp','r');
formatSpec = '%d %f %f %f %f';
sizeA = [5 Inf];
A = fscanf(fileID,formatSpec,sizeA);
B = A(2:5, :);

B(1, :) = A(4, :);
B(2, :) = A(5, :);
B(3, :) = A(2, :);
B(4, :) = A(3, :);

ColList='ymcrgb'; %no w letter because you won't see the white line 


    
for i = 1:30
   col=randi([1 6]);
   h = rectangle('Position',B(:,i),'FaceColor',ColList(col));
   txt1 = num2str(i);
   text(B(1,i)+(B(3,i)/2),B(2,i)+(B(4,i)/2),txt1)
   axis([0 0.035 0 0.01])
end

fclose(fileID);

