#pragma version(1)
#pragma rs java_package_name(aexp.samplerecognizer)

int32_t s1len = 0;
int32_t s2len = 0;
int32_t *d0;
int32_t *d1;
int16_t *signal1;
int16_t *signal2;

static int32_t signalCost( int32_t x1, int32_t x2 ) {
	return x1 > x2 ? x1 - x2 : x2 - x1;
}

static int32_t int32_min( int32_t x1, int32_t x2 ) {
	return x1 > x2 ? x2 : x1;
}

void dtw() {
		int32_t s = 0;
		rsDebug( "s1len",s1len);
		rsDebug( "s2len",s2len);
		for( int32_t x = 0 ; x < s1len ; ++x ) {
			s += signalCost((int32_t)signal1[x],(int32_t)signal2[0]);
			d0[x] = s;
		}
		s = 0;
		for( int32_t y = 1 ; y < s2len ; ++y ) {
			s += signalCost((int32_t)signal1[0],(int32_t)signal2[y]);
			d1[0] = s;
			for( int32_t x = 1 ; x < s1len ; ++x ) {
				int cs = signalCost( (int32_t)signal1[x],(int32_t)signal2[y]);
				int32_t m = 
					int32_min( 
						d0[x-1],
						d1[x-1] );
				m = int32_min( m, d0[x] );
				d1[x] = cs+m;
			}
			for( int32_t x = 0 ; x < s1len ; ++x )
				d0[x] = d1[x];
		}
		rsDebug( "maxCost", d1[s1len-1] );
}

