
#ifdef WIN32
#include <windows.h>
#endif

#include "jdkmidi/world.h"
#include "jdkmidi/track.h"
#include "jdkmidi/multitrack.h"
#include "jdkmidi/filereadmultitrack.h"
#include "jdkmidi/fileread.h"
#include "jdkmidi/fileshow.h"
#include "jdkmidi/filewritemultitrack.h"

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::lexical_cast;

int main(int argc, char *argv[])
{
	int return_code = -1;

	if(argc > 2) {
		const string infile_name = argv[1];
		const string outfile_name = argv[2];

		jdkmidi::MIDIFileReadStreamFile rs( infile_name.c_str() );

		jdkmidi::MIDIMultiTrack tracks;

		jdkmidi::MIDIFileReadMultiTrack track_loader( &tracks );

		jdkmidi::MIDIFileRead reader( &rs, &track_loader );

		reader.Parse();

		for (int i = 1; i < reader.GetNumberTracks(); ++i) {
			string number;
			
			if (i < 10)
				number = string("0") + lexical_cast<string>(i);
			else
				number = lexical_cast<string>(i);

			jdkmidi::MIDIFileWriteStreamFileName out_stream( string(
				outfile_name.substr(0, outfile_name.size() - 4) + number + string(".mid")
				).c_str());

			if(out_stream.IsValid()) {
				bool f = true;

				jdkmidi::MIDIFileWrite writer(&out_stream);
				writer.WriteFileHeader(1, 1, reader.GetDivision());

				if( writer.ErrorOccurred() )
				{
					f=false;
					break;
				}
      
				const jdkmidi::MIDITrack *t = tracks.GetTrack(i);
      
				jdkmidi::MIDIClockTime last_event_time=0;
      
				writer.WriteTrackHeader(0); // will be rewritten later
      
				if( t )
				{
					for( int event_num=0; event_num<t->GetNumEvents(); ++event_num )
					{
						const jdkmidi::MIDITimedBigMessage *ev = t->GetEventAddress( event_num );
						if( ev && !ev->IsNoOp() )
						{
							last_event_time = ev->GetTime();
            
							if( !ev->IsDataEnd() )
							{
								writer.WriteEvent( *ev );
              
								if( writer.ErrorOccurred() )
								{
									f=false;
									break;
								}
							}
						}
					}
				}
				writer.WriteEndOfTrack(0);
				writer.RewriteTrackLength();
			}
			else
			{
				fprintf( stderr, "Error opening file '%s'\n", outfile_name.c_str() );
			}
		}
	}
	else
	{
		fprintf( stderr, "usage:\n\tmidi_track_extractor INFILE.mid OUTFILE.mid\n" );
	}

	return return_code;		
}