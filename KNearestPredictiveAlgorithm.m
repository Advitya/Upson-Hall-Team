function [ predicted ] = predict ( measured )
 [mr, mc] = size(measured);
 predicted = zeros(mr, 1);
for n = 1:1:mr/6
  predicted(1:mr/6,1) = measured(1:mr/6, 1);
end

sum =0;

vt =1.6;
%one day => 97 (each 15 min)
%one week => 679
for a = 2:1:6
  for b = 1:1:7
    for c =1:1:7
      ind = (a-1)*679 + (b-1)*97 +c+ 1;
      if a==2
        for p =-1:1:4
          sum=sum + measured(ind-679+p)/ (p+2);
        end
      else
        for t = 1:1:a-1
          for p = -1:1:4
            sum = sum + measured(ind-679*t+p)/abs(t^2*(p+2);
          end
        end
      end
      if sum < vt
        predicted(ind,1) = 0;
      else
        predicte(ind,1) = 1;
      end
      sum = 0
    end
  end
end

end
