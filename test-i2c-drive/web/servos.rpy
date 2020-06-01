import smbus
import time
bus = smbus.SMBus(1)
address = 0x04
from twisted.web.resource import Resource

class MoveServo(Resource):
        isLeaf = True
        def render_GET(self,request):
                try:
                		# Send value over i2c to arduino
                        bus.write_byte(address, int(request.args['value'][0]))
                        return 'Success'
                except:
                        return 'Failure'

resource = MoveServo()
